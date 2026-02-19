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
    ];

    postPatch = ''
      sed -i -e 's/0.0.0/${version}/' configure.ac
    '';
  };
in
{
  package = xcmp;

  devShell = pkgs.mkShell {
    inputsFrom = [
      xcmp
    ];

    buildInputs = with pkgs; [
      clang-tools
    ];
  };
}
