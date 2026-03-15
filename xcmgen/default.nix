{ pkgs }:
let
  pname = "xcmgen";
  version = "0.0.0";

  pypkgs = pkgs.python313Packages;

  xcmgen = pkgs.stdenv.mkDerivation {
    inherit pname version;
    src = ./.;

    buildInputs = with pypkgs; [
      (python.withPackages (pypkgs: [
        jinja2
        toml
      ]))
    ];
  };
in
rec {
  package = xcmgen;

  devShell = pkgs.mkShell {
    inputsFrom = [
      xcmgen
    ];

    buildInputs = with pypkgs; [
      python-lsp-server
      python-lsp-ruff
      ruff
    ];
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
  };
}
