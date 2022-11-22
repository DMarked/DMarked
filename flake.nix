{
  description = "A very basic flake";
  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    dde-nixos = {
      url = "github:linuxdeepin/dde-nixos";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = { self, flake-utils, dde-nixos, nixpkgs }@input:
    flake-utils.lib.eachSystem [ "x86_64-linux" "aarch64-linux" "i686-linux" ]
      (system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
          
          dde-pkgs = dde-nixos.packages.${system};

          dmarked = pkgs.callPackage ./nix {
            inherit dde-pkgs;
          };
        in
        rec {
          packages.default = dmarked;

          apps.${system}.default = {
            type = "app";
            program = "${self.packages.${system}.default}/bin/dmarked";
          };

          devShell = pkgs.mkShell {
            nativeBuildInputs = with pkgs; [ cmake pkg-config ];
            buildInputs = with dde-pkgs; [ dtkcore dtkgui dtkwidget ] ++ (with pkgs; 
            [ qmarkdowntextedit ]);

            shellHook = ''
              # export QT_LOGGING_RULES=*.debug=true
              export QT_PLUGIN_PATH="${dde-pkgs.qt5integration}/lib/qt-5.15.5/plugins:$QT_PLUGIN_PATH"
              export QT_QPA_PLATFORM_PLUGIN_PATH="${dde-pkgs.qt5platform-plugins}/lib/qt-5.15.5/plugins"
            '';
          };
        }
      );
}
