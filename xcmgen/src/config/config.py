from enum import Enum
from pydantic import BaseModel, PrivateAttr, model_validator
from typing import Optional, Self


class NodeModel:
    pass


class ConfigModel:
    pass


class CollectionTypeEnum(str, Enum):
    SINGLE = "single"
    VECTOR = "vec"


class StorageSemanticsEnum(str, Enum):
    PLAIN = "plain"
    REFERENCE = "ref"
    POINTER = "ptr"
    UNIQUE_POINTER = "uptr"


class TypeModel(BaseModel):
    root_type: str
    collection_type: CollectionTypeEnum = CollectionTypeEnum.SINGLE
    storage_semantics: StorageSemanticsEnum = StorageSemanticsEnum.PLAIN

    _node_alias: bool = PrivateAttr(False)

    _node: NodeModel = PrivateAttr(None)
    _config: ConfigModel = PrivateAttr(None)

    def set_node(self, node: NodeModel):
        self._node = node
        self._config = node._config

    def is_node(self) -> bool:
        if self.root_type == "Ast":
            return True

        for node in self._config.nodes:
            if self.root_type == node.name:
                return True

        return False

    def box_storage_semantics(self, type: str) -> str:
        match self.storage_semantics:
            case StorageSemanticsEnum.PLAIN:
                return type
            case StorageSemanticsEnum.REFERENCE:
                return f"{type}&"
            case StorageSemanticsEnum.POINTER:
                return f"{type}*"
            case StorageSemanticsEnum.UNIQUE_POINTER:
                if not self._node_alias and self.is_node():
                    return f"{type}::UPtr"

                return f"std::unique_ptr<{type}>"

    def box_collection_type(self, type: str) -> str:
        match self.collection_type:
            case CollectionTypeEnum.SINGLE:
                return type
            case CollectionTypeEnum.VECTOR:
                return f"std::vector<{type}>"

    def is_alias(self) -> bool:
        for alias in self._node.aliases:
            if self.root_type == alias.name:
                return True

        return False

    def retrieve_alias(self) -> Self:
        for alias in self._node.aliases:
            if self.root_type == alias.name:
                return alias.type

        return self

    def store_type(self, include_namespace: bool = False) -> str:
        namespace = self.namespace if include_namespace else ""
        store_type = f"{namespace}{self.root_type}"
        store_type = self.box_storage_semantics(store_type)
        store_type = self.box_collection_type(store_type)
        return store_type

    def is_primitive_type(self) -> bool:
        type = self.retrieve_alias()
        if type.collection_type != CollectionTypeEnum.SINGLE:
            return False

        if type.storage_semantics != StorageSemanticsEnum.PLAIN:
            return False

        if type.root_type == "bool":
            return True

        return False

    def is_optional(self) -> bool:
        type = self.retrieve_alias()
        if type.collection_type != CollectionTypeEnum.SINGLE:
            return False

        if type.storage_semantics == StorageSemanticsEnum.POINTER:
            return True

        return False

    def r_type(self, include_namespace: bool = False) -> str:
        namespace = self.namespace if include_namespace else ""
        type = self.retrieve_alias()
        if type.collection_type != CollectionTypeEnum.SINGLE:
            r_type = self.store_type(include_namespace)
            r_type = f"const {r_type}&"
            return r_type

        r_type = self.root_type
        if self.is_primitive_type():
            return r_type

        if self.is_optional():
            return f"const {namespace}{r_type}*"
        else:
            return f"const {namespace}{r_type}&"

    def w_type(self, include_namespace: bool = False) -> str:
        namespace = self.namespace if include_namespace else ""
        type = self.retrieve_alias()
        if type.collection_type != CollectionTypeEnum.SINGLE:
            w_type = self.store_type(include_namespace)
            w_type = f"{w_type}&"
            return w_type

        w_type = self.root_type
        if self.is_optional():
            return f"{namespace}{w_type}*"
        else:
            return f"{namespace}{w_type}&"

    def W_type(self) -> str:
        type = self.retrieve_alias()
        if type.collection_type != CollectionTypeEnum.SINGLE:
            W_type = self.store_type()
            if type.storage_semantics == StorageSemanticsEnum.PLAIN:
                W_type = f"const {W_type}&"
            return W_type

        if type.storage_semantics == StorageSemanticsEnum.UNIQUE_POINTER:
            return self.store_type()

        W_type = self.store_type()
        if not self.is_primitive_type():
            W_type = f"const {W_type}&"

        return W_type

    def need_deref(self) -> bool:
        if self.collection_type != CollectionTypeEnum.SINGLE:
            return False

        return self.storage_semantics == StorageSemanticsEnum.UNIQUE_POINTER

    def requires_move(self) -> bool:
        type = self.retrieve_alias()
        return type.storage_semantics == StorageSemanticsEnum.UNIQUE_POINTER

    @property
    def namespace(self) -> str:
        if self.is_alias():
            return f"{self._node.name}::"

        return ""


class AliasModel(BaseModel):
    name: str
    type: TypeModel

    _node: NodeModel = PrivateAttr(None)
    _config: ConfigModel = PrivateAttr(None)

    def set_node(self, node: NodeModel):
        self._node = node
        self._config = node._config
        self.type.set_node(node)


class MemberModel(BaseModel):
    name: str
    type: TypeModel
    ctor: bool = True
    r_acc: bool = False
    w_acc: bool = False
    W_acc: bool = False
    default_value: Optional[bool | int | str] = None

    _node: NodeModel = PrivateAttr(None)
    _config: ConfigModel = PrivateAttr(None)

    def set_node(self, node: NodeModel):
        self._node = node
        self._config = node._config
        self.type.set_node(node)

    @property
    def default(self) -> str:
        if self.default_value is None:
            return None

        if isinstance(self.default_value, bool):
            return "true" if self.default_value else "false"

        if isinstance(self.default_value, str):
            return f'"{self.default_value}"'

        return self.default_value


class NodeModel(BaseModel):
    name: str
    sys_includes: list[str] = []
    includes: list[str] = []
    local_includes: list[str] = []
    aliases: list[AliasModel] = []
    members: list[MemberModel] = []

    _config: ConfigModel = PrivateAttr(None)

    def set_config(self, config: ConfigModel):
        self._config = config
        for member in self.members:
            member.set_node(self)

        for alias in self.aliases:
            alias.set_node(self)

    @model_validator(mode="after")
    def add_uptr_alias(self) -> Self:
        if all(alias.name != "UPtr" for alias in self.aliases):
            node_alias = AliasModel(
                name="UPtr",
                type=TypeModel(
                    root_type=self.name,
                    storage_semantics="uptr",
                ),
            )
            node_alias.type._node_alias = True
            self.aliases = [
                node_alias,
                *self.aliases,
            ]
        return self


class ConfigModel(BaseModel):
    nodes: list[NodeModel] = []

    @model_validator(mode="after")
    def set_nodes_config(self) -> Self:
        for node in self.nodes:
            node.set_config(self)
        return self
