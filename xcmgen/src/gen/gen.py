from jinja2 import Environment, PackageLoader, select_autoescape

from utils import name_utils


def generate():
    env = Environment(
        loader=PackageLoader("gen"),
        autoescape=select_autoescape(),
    )
    env.filters["kebab"] = name_utils.kebab
    node_hh_template = env.get_template("node.hh.jinja")
    node = {
        "name": "Text",
        "sys_includes": [
            "<string>",
        ],
        "includes": [],
        "local_includes": [
            "ast.hh",
        ],
        "members": [
            {
                "name": "text",
                "store_type": "std::string",
                "r_type": "const std::string&",
                "w_type": "std::string&",
                "W_type": "const std::string&",
                "ctor": True,
                "r_acc": True,
                "w_acc": False,
                "W_acc": False,
                "default_value": None,
            },
        ],
    }
    print(node_hh_template.render(node=node))
