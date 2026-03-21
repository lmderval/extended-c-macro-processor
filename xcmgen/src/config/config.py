from pydantic import BaseModel
from typing import Any


class AliasModel(BaseModel):
    name: str
    type: str


class MemberModel(BaseModel):
    name: str
    store_type: str
    r_type: str
    w_type: str
    W_type: str
    ctor: bool = True
    r_acc: bool = False
    w_acc: bool = False
    W_acc: bool = False
    default_value: Any = None


class NodeModel(BaseModel):
    name: str
    sys_includes: list[str] = []
    includes: list[str] = []
    local_includes: list[str] = []
    aliases: list[AliasModel] = []
    members: list[MemberModel] = []


class ConfigModel(BaseModel):
    nodes: list[NodeModel] = []
