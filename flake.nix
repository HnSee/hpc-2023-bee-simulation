{
  description = "Project as part of the course \"Practical Course on High-Performance Computing\" (summer term 2023).";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  # Needs sandbox to be disabled because of meson's wrap dependencies
  # Can be disabled while building using:
  #   nix build --no-sandbox

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      rec {
        packages = {
          bee-simulation = pkgs.stdenv.mkDerivation rec {
            name = "bee-simulation";
            src = self;

            nativeBuildInputs = with pkgs; [
              meson
              ninja
              pkg-config
              cacert
            ];

            buildInputs = with pkgs; [
              openmpi
              fmt
              cairomm
              cxxopts
              spdlog
            ];

            mesonWrapMode = "default";
          };
        };

        defaultPackage = packages.bee-simulation;
      });
}
