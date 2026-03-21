from enum import Enum
from pydantic import BaseModel, model_validator
from typing import Optional, Self


class AliasModel(BaseModel):
    name: str
    type: str


class CollectionTypeEnum(str, Enum):
    SINGLE = "single"
    VECTOR = "vec"


class StorageSemanticsEnum(str, Enum):
    PLAIN = "plain"
    REFERENCE = "ref"
    POINTER = "ptr"
    UNIQUE_POINTER = "uptr"


class MemberModel(BaseModel):
    name: str
    root_type: str
    collection_type: CollectionTypeEnum = CollectionTypeEnum.SINGLE
    storage_semantics: StorageSemanticsEnum = StorageSemanticsEnum.PLAIN
    ctor: bool = True
    r_acc: bool = False
    w_acc: bool = False
    W_acc: bool = False
    default_value: Optional[bool | int | str] = None

    def box_storage_semantics(self, type: str) -> str:
        match self.storage_semantics:
            case StorageSemanticsEnum.PLAIN:
                return type
            case StorageSemanticsEnum.REFERENCE:
                return f"{type}&"
            case StorageSemanticsEnum.POINTER:
                return f"{type}*"
            case StorageSemanticsEnum.UNIQUE_POINTER:
                return f"std::unique_ptr<{type}>"

    def box_collection_type(self, type: str) -> str:
        match self.collection_type:
            case CollectionTypeEnum.SINGLE:
                return type
            case CollectionTypeEnum.VECTOR:
                return f"std::vector<{type}>"

    @property
    def store_type(self) -> str:
        store_type = self.root_type
        store_type = self.box_storage_semantics(store_type)
        store_type = self.box_collection_type(store_type)
        return store_type

    def is_primitive_type(self) -> bool:
        if self.root_type == "bool":
            return True

        return False

    def is_optional(self) -> bool:
        if self.collection_type != CollectionTypeEnum.SINGLE:
            return False

        if self.storage_semantics == StorageSemanticsEnum.POINTER:
            return True

        return False

    @property
    def r_type(self) -> str:
        if self.collection_type != CollectionTypeEnum.SINGLE:
            r_type = self.store_type
            r_type = f"const {r_type}&"
            return r_type

        r_type = self.root_type
        if (
            self.storage_semantics == StorageSemanticsEnum.PLAIN
            and self.is_primitive_type()
        ):
            return r_type

        if self.is_optional():
            return f"const {r_type}*"
        else:
            return f"const {r_type}&"

    @property
    def w_type(self) -> str:
        if self.collection_type != CollectionTypeEnum.SINGLE:
            w_type = self.store_type
            w_type = f"{w_type}&"
            return w_type

        w_type = self.root_type
        if self.is_optional():
            return f"{w_type}*"
        else:
            return f"{w_type}&"

    @property
    def W_type(self) -> str:
        if self.collection_type != CollectionTypeEnum.SINGLE:
            W_type = self.store_type
            if self.storage_semantics == StorageSemanticsEnum.UNIQUE_POINTER:
                W_type = f"{W_type}&&"
            return W_type

        if self.storage_semantics == StorageSemanticsEnum.UNIQUE_POINTER:
            W_type = self.store_type
            return f"{W_type}&&"

        W_type = self.store_type
        if (
            self.storage_semantics == StorageSemanticsEnum.PLAIN
            and not self.is_primitive_type()
        ):
            W_type = f"const {W_type}&"

        return W_type

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

    @model_validator(mode="after")
    def add_uptr_alias(self) -> Self:
        if all(alias.name != "UPtr" for alias in self.aliases):
            self.aliases = [
                AliasModel(name="UPtr", type=f"std::unique_ptr<{self.name}>"),
                *self.aliases,
            ]
        return self


class ConfigModel(BaseModel):
    nodes: list[NodeModel] = []
