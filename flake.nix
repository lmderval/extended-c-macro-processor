{
  description = "A flake for extended C macro procesor";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-25.11";
    systems.url = "github:nix-systems/x86_64-linux";
    flake-utils = {
      url = "github:numtide/flake-utils";
      inputs.systems.follows = "systems";
    };
    pyproject-nix = {
      url = "github:pyproject-nix/pyproject.nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    uv2nix = {
      url = "github:pyproject-nix/uv2nix";
      inputs.pyproject-nix.follows = "pyproject-nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    pyproject-build-systems = {
      url = "github:pyproject-nix/build-system-pkgs";
      inputs.pyproject-nix.follows = "pyproject-nix";
      inputs.uv2nix.follows = "uv2nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };
  outputs =
    { self
    , nixpkgs
    , flake-utils
    , uv2nix
    , pyproject-nix
    , pyproject-build-systems
    , ...
    }: (
      flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        pytools = {
          inherit uv2nix;
          py-nix = pyproject-nix;
          py-build = pyproject-build-systems;
        };
        xcmp = import ./xcmp { inherit pkgs; };
        xcmgen = import ./xcmgen { inherit pkgs pytools; };
      in
      {
        packages = {
          xcmp = xcmp.package;
          xcmgen = xcmgen.package;
          default = xcmp.package;
        };

        devShells = {
          xcmp = xcmp.devShell;
          xcmgen = xcmgen.devShell;
          default = xcmp.devShell;
        };

        images = {
          xcmp = xcmp.image;
          xcmgen = xcmgen.image;
        };

        formatter = pkgs.nixpkgs-fmt;
      })
    );
}
