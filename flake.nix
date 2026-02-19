{
  description = "A flake for extended C macro procesor";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-25.11";
    systems.url = "github:nix-systems/x86_64-linux";
    flake-utils = {
      url = "github:numtide/flake-utils";
      inputs.systems.follows = "systems";
    };
  };
  outputs =
    { self
    , nixpkgs
    , flake-utils
    , ...
    }: (
      flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        xcmp = import ./xcmp { inherit pkgs; };
      in
      {
        packages = {
          xcmp = xcmp.package;
          default = xcmp.package;
        };

        devShells = {
          xcmp = xcmp.devShell;
          default = xcmp.devShell;
        };

        formatter = pkgs.nixpkgs-fmt;
      })
    );
}
