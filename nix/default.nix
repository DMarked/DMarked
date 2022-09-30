{ stdenv
, lib
, fetchFromGitHub
, dtk
, qt5integration
, qt5platform-plugins
, qmake
, qttools
, pkg-config
, qtwebengine
, wrapQtAppsHook
, qtbase
}:
stdenv.mkDerivation rec {
  pname = "dmarked";
  version = "0.2.0";

  src = ./..;

  nativeBuildInputs = [
    cmake
    qttools
    pkg-config
    wrapQtAppsHook
  ];

  buildInputs = [
    dtk
    qtwebengine
  ];

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

