{
  description = "Project as part of the course \"Practical Course on High-Performance Computing\" (summer term 2023).";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        spdlog_fix = final: prev: {
          spdlog = prev.spdlog.overrideAttrs (old: {
            version = "1.12.0";
            src = final.fetchFromGitHub {
              owner = "gabime";
              repo = "spdlog";
              rev = "v1.12.0";
              hash = "sha256-cxTaOuLXHRU8xMz9gluYz0a93O0ez2xOxbloyc1m1ns=";
            };
            patches = [
              # Fix a broken test
              (final.fetchpatch {
                url = "https://github.com/gabime/spdlog/commit/2ee8bac78e6525a8ad9a9196e65d502ce390d83a.patch";
                hash = "sha256-L79yOkm3VY01jmxNctfneTLmOA5DEQeNNGC8LbpJiOc=";
              })
            ];
            propagatedBuildInputs = [ final.fmt ];
            checkInputs = [ final.catch2_3 ];
          });
        };
        pkgs = nixpkgs.legacyPackages.${system}.extend spdlog_fix;
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
