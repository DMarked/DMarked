{
  description = "A very basic flake";

  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, flake-utils, nixpkgs }@input:
    flake-utils.lib.eachSystem [ "x86_64-linux" "aarch64-linux" "i686-linux" ]
      (system:
        let
          pkgs = nixpkgs.legacyPackages.${system};

          dmarked = pkgs.deepin.callPackage ./nix { };
        in
        rec {
          packages.default = dmarked;

          apps.${system}.default = {
            type = "app";
            program = "${self.packages.${system}.default}/bin/dmarked";
          };

          devShell = pkgs.mkShell {
            nativeBuildInputs = with pkgs; [ cmake pkg-config ];
            buildInputs = with pkgs; with deepin; [
              dtkwidget
              qmarkdowntextedit
            ];

            shellHook = ''
              # export QT_LOGGING_RULES=*.debug=true
            '';
          };
        }
      );
}
