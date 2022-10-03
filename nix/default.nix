{ stdenv
, lib
, fetchFromGitHub
, dde-pkgs
, libsForQt5
, cmake
, pkg-config
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

  buildInputs = with dde-pkgs; [
    dtkcore
    dtkgui
    dtkwidget
    qt5platform-plugins
    libsForQt5.qtwebengine
  ];

  qtWrapperArgs = [
    "--prefix QT_QPA_PLATFORM_PLUGIN_PATH : ${dde-pkgs.qt5platform-plugins}/${libsForQt5.qtbase.qtPluginPrefix}"
    "--prefix QT_PLUGIN_PATH : ${dde-pkgs.qt5integration}/${libsForQt5.qtbase.qtPluginPrefix}"
  ];

  meta = with lib; {
    description = "dtk based markdown editor";
    homepage = "https://github.com/DMarked/DMarked";
    license = licenses.gpl3Plus;
    platforms = platforms.linux;
  };
}

