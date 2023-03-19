{ stdenv
, lib
, fetchFromGitHub
, libsForQt5
, cmake
, pkg-config
, deepin
}:
stdenv.mkDerivation rec {
  pname = "dmarked";
  version = "0.3.0";

  src = ./..;

  nativeBuildInputs = with libsForQt5; [
    cmake
    qttools
    pkg-config
    wrapQtAppsHook
  ];

  buildInputs = with deepin; [
    dtkwidget
    qt5platform-plugins
    libsForQt5.qtwebengine
  ];

  qtWrapperArgs = [
    "--prefix QT_PLUGIN_PATH : ${deepin.qt5integration}/${libsForQt5.qtbase.qtPluginPrefix}"
  ];

  meta = with lib; {
    description = "dtk based markdown editor";
    homepage = "https://github.com/DMarked/DMarked";
    license = licenses.gpl3Plus;
    platforms = platforms.linux;
  };
}

