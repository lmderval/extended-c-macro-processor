{ pkgs }:
let
  pname = "xcmp";
  version = "0.0.0";
  xcmp = pkgs.stdenv.mkDerivation {
    inherit pname version;
    src = ./.;

    nativeBuildInputs = with pkgs; [
      autoconf
      autoconf-archive
      autoreconfHook
      automake
      libtool
      pkg-config
      flex
      bison
    ];

    postPatch = ''
      sed -i -e 's/0.0.0/${version}/' configure.ac
    '';
  };
in
rec {
  package = xcmp;

  devShell = pkgs.mkShell {
    inputsFrom = [
      xcmp
    ];

    buildInputs = with pkgs; [
      clang-tools
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
