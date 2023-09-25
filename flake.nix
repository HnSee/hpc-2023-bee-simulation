{
  description = "Project as part of the course \"Practical Course on High-Performance Computing\" (summer term 2023).";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      rec {
        packages = {
          bee-simulation = pkgs.stdenv.mkDerivation {
            name = "bee-simulation";
            src = self;

            nativeBuildInputs = with pkgs; [
              meson
              ninja
              pkg-config
            ];

            buildInputs = with pkgs; [
              cairomm
              cxxopts
              gbenchmark
              gtest
              openmpi
              spdlog
            ];
          };
          
          bee-simulation-cmake = pkgs.stdenv.mkDerivation {
            name = "bee-simulation-cmake";
            src = self;

            nativeBuildInputs = with pkgs; [
              cmake
              pkg-config
            ];

            buildInputs = with pkgs; [
              cairomm
              cxxopts
              gbenchmark
              gtest
              openmpi
              spdlog
            ];
          };
        };

        defaultPackage = packages.bee-simulation;
      });
}
