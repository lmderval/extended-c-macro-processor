{ pkgs, pytools }:
let
  workspace = pytools.uv2nix.lib.workspace.loadWorkspace {
    workspaceRoot = ./.;
  };

  python = pkgs.python313;

  overlay = workspace.mkPyprojectOverlay {
    sourcePreference = "wheel";
  };

  pythonSet =
    (pkgs.callPackage pytools.py-nix.build.packages {
      inherit python;
    }).overrideScope
      (
        pkgs.lib.composeManyExtensions [
          pytools.py-build.overlays.default
          overlay
        ]
      );

  virtualenv = pythonSet.mkVirtualEnv "xcmgen-env" workspace.deps.default;

  xcmgen-pyproject = pytools.py-nix.lib.project.loadPyproject {
    projectRoot = ./.;
  };

  xcmgen-attrs = xcmgen-pyproject.renderers.buildPythonPackage {
    inherit python;
  };

  xcmgen = python.pkgs.buildPythonPackage xcmgen-attrs;

  pname = xcmgen.pname;
  version = xcmgen.version;
in
rec {
  package = xcmgen;

  devShell = pkgs.mkShell {
    buildInputs = [
      pkgs.uv
      virtualenv
    ];

    env = {
      UV_PYTHON = "${virtualenv}/bin/python";
      UV_PYTHON_DOWNLOADS = "never";
    };
  };

  image = pkgs.dockerTools.buildImage {
    name = pname;
    tag = version;
    copyToRoot = pkgs.buildEnv {
      name = "image-root";
      pathsToLink = [ "/bin" ];
      paths = with pkgs; [
        bash
        coreutils
      ] ++ (
        devShell.nativeBuildInputs ++ devShell.buildInputs
      );
    };
    config = {
      Env = [
        "UV_PYTHON=${virtualenv}/bin/python"
      ];
    };
  };
}
