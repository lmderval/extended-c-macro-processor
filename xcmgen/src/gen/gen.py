from argparse import ArgumentParser
from jinja2 import Environment, PackageLoader, select_autoescape
import toml

from config.config import ConfigModel

from utils import name_utils


def create_argument_parser() -> ArgumentParser:
    parser = ArgumentParser(
        prog="xcmgen",
        description="AST Node generator for xcmp",
    )
    parser.add_argument("-c", "--config", type=str)
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


def generate():
    parser = create_argument_parser()
    args = parser.parse_args()
    config = read_config(args.config)
    env = create_environment()
    node_hh_template = env.get_template("node.hh.jinja")
    for node in config.nodes:
        print(node_hh_template.render(node=node))
