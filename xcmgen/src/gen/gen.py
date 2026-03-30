from argparse import ArgumentParser
from enum import Enum
from jinja2 import Environment, PackageLoader, Template, select_autoescape
import sys
import toml
from typing import Optional

from config.config import ConfigModel, NodeModel

from utils import name_utils


class TemplateEnum(str, Enum):
    HEADER = "hh"
    INLINE = "hxx"
    IMPLEM = "cc"


def create_argument_parser() -> ArgumentParser:
    parser = ArgumentParser(
        prog="xcmgen",
        description="AST Node generator for xcmp",
    )
    parser.add_argument("-c", "--config", type=str)
    parser.add_argument("-o", "--output", type=str)
    parser.add_argument("-f", "--file", type=str, required=False, default=None)
    return parser


def create_environment() -> Environment:
    env = Environment(
        loader=PackageLoader("gen"),
        autoescape=select_autoescape(),
        trim_blocks=True,
        lstrip_blocks=True,
    )
    env.filters["kebab"] = name_utils.kebab
    return env


def read_config(config_path: str) -> ConfigModel:
    config = toml.load((config_path))
    return ConfigModel.model_validate(config)


def render_node(
    node: NodeModel,
    template_map: dict[TemplateEnum, Template],
    output: str,
    template: Optional[TemplateEnum] = None,
):
    basename = name_utils.kebab(node.name)
    templates = [template] if template else list(template_map.keys())
    for template in templates:
        filename = f"{output}/{basename}.{template.value}"
        content = template_map[template].render(node=node).strip() + "\n"
        with open(filename, mode="w") as file:
            file.write(content)


def generate():
    parser = create_argument_parser()
    args = parser.parse_args()
    config = read_config(args.config)
    env = create_environment()

    template_map = {
        template: env.get_template(f"node.{template.value}.jinja")
        for template in [
            TemplateEnum.HEADER,
            TemplateEnum.INLINE,
            TemplateEnum.IMPLEM,
        ]
    }

    node: Optional[NodeModel] = None
    template: Optional[TemplateEnum] = None

    if args.file:
        file = args.file.split(".")
        if len(file) == 0 or len(file) > 2:
            print(f"Invalid file format: '{args.file}'", file=sys.stderr)
            sys.exit(1)

        for node in config.nodes:
            basename = name_utils.kebab(node.name)
            if basename == file[0]:
                break
        else:
            print(
                f"Node does not exist: '{file[0]}' extracted from file '{args.file}'",
                file=sys.stderr,
            )
            sys.exit(1)

        if len(file) == 2:
            for template in template_map:
                if template.value == file[1]:
                    break
            else:
                print(
                    f"Invalid template: '{file[1]}' extracted from file '{args.file}'",
                    file=sys.stderr,
                )
                sys.exit(1)

    nodes = [node] if node else config.nodes
    for node in nodes:
        render_node(node, template_map, args.output, template)
