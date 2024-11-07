{ stdenv
, lib
, fetchFromGitHub
, cmake
, qttools
, pkg-config
, wrapQtAppsHook
, qtbase
, dtkwidget
, qt5integration
, qt5platform-plugins
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
    dtkwidget
    qt5platform-plugins
    qtwebengine
    qmarkdowntextedit
    fakevim
  ];

  cmakeFlags = [
    "-DUES_VENDORED_QMARKDOWNTEXTEDIT=OFF"
    "-DUES_VENDORED_FAKEVIM=OFF"
  ];

  strictDeps = true;

  qtWrapperArgs = [
    "--prefix QT_PLUGIN_PATH : ${qt5integration}/${qtbase.qtPluginPrefix}"
  ];

  meta = with lib; {
    description = "dtk based markdown editor";
    homepage = "https://github.com/DMarked/DMarked";
    license = licenses.gpl3Plus;
    platforms = platforms.linux;
  };
}
