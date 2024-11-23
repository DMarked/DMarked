{ stdenv
, lib
, fetchFromGitHub
, cmake
, qttools
, pkg-config
, wrapQtAppsHook
, qtbase
, deepin
, qtwebengine
, qmarkdowntextedit
, fakevim
}:

stdenv.mkDerivation rec {
  pname = "dmarked";
  version = "0.3.0";

  src = ./..;

  nativeBuildInputs = [
    cmake
    qttools
    pkg-config
    wrapQtAppsHook
  ];

  buildInputs = [
    qtbase
    deepin.dtkwidget
    deepin.qt5platform-plugins
    deepin.qt5integration
    qtwebengine
    qmarkdowntextedit
    fakevim
  ];

  cmakeFlags = [
    "-DUES_VENDORED_QMARKDOWNTEXTEDIT=OFF"
    "-DUES_VENDORED_FAKEVIM=OFF"
  ];

  strictDeps = true;

  meta = {
    description = "dtk based markdown editor";
    homepage = "https://github.com/DMarked/DMarked";
    license = lib.licenses.gpl3Plus;
    platforms = lib.platforms.linux;
    maintainers = with lib.maintainers; [ rewine ];
  };
}
