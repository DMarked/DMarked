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

          fakevim = pkgs.libsForQt5.callPackage ./nix/fakevim.nix { };
          dmarked = pkgs.libsForQt5.callPackage ./nix { inherit fakevim; };
        in
        rec {
          packages.default = dmarked;

          apps.${system}.default = {
            type = "app";
            program = "${self.packages.${system}.default}/bin/dmarked";
          };

          devShell = pkgs.mkShell {
            inputsFrom = [ dmarked ];

            buildInputs = with pkgs; [
              deepin.dtkwidget
              qmarkdowntextedit
            ];

            shellHook = let
              qtPkgs = pkgs.libsForQt5;
              makeQtpluginPath = pkgs.lib.makeSearchPathOutput "out" pkgs.qt6.qtbase.qtPluginPrefix;
              makeQmlpluginPath = pkgs.lib.makeSearchPathOutput "out" pkgs.qt6.qtbase.qtQmlPrefix;
            in
            ''
              export QT_PLUGIN_PATH=${makeQtpluginPath (with qtPkgs; with pkgs.deepin; [ qtbase dtkwidget qtwayland qt5platform-plugins qt5integration ])}
              # export QT_LOGGING_RULES=*.debug=true
            '';
          };
        }
      );
}
