CurrentDir=$(pwd)
BuildDebugDir="${CurrentDir}/../build/sdks-debug/";
BuildDir="${CurrentDir}/../build/sdks/";
CmakeDebug="cmake -DCMAKE_BUILD_TYPE=Debug --build --clean-first ${CurrentDir}";
CmakeRelease="cmake -DCMAKE_BUILD_TYPE=Release --build --clean-first ${CurrentDir}";
Make="make -j4";
MakeInstall="make install";
Log="build.log";
LogFile="${CurrentDir}/$Log";

echo "Current dir: ${CurrentDir}"
echo "Current log: ${LogFile}"
echo ""

echo "Build-Log for external libraries" &> "$LogFile"
echo "" &>> "$LogFile"

cd "../"
mkdir -p "build/sdks"
mkdir -p "build/sdks-debug"
cd "sdks-source/"


cd ${BuildDebugDir}

echo "Executing Cmake for debug build..."
echo "------------------------------" &>> "$LogFile"
echo "Cmake Debug Log:" &>> "$LogFile"
echo "------------------------------" &>> "$LogFile"
$CmakeDebug &>> "$LogFile"

echo "Building debug..."
echo "------------------------------" &>> "$LogFile"
echo "Compile Debug Log:" &>> "$LogFile"
echo "------------------------------" &>> "$LogFile"
$Make &>> "$LogFile"

echo "Installing debug..."
echo "------------------------------" &>> "$LogFile"
echo "Make Install Debug Log:" &>> "$LogFile"
echo "------------------------------" &>> "$LogFile"
$MakeInstall &>> "$LogFile"

cd ${BuildDir}

echo ""
echo "Executing Cmake for release build..."
echo "------------------------------" &>> "$LogFile"
echo "Cmake Release Log:" &>> "$LogFile"
echo "------------------------------" &>> "$LogFile"
$CmakeRelease &>> "$LogFile"

echo "Building release..."
echo "------------------------------" &>> "$LogFile"
echo "Compile Release Log:" &>> "$LogFile"
echo "------------------------------" &>> "$LogFile"
$Make &>> "$LogFile"

echo "Installing release..."
echo "------------------------------" &>> "$LogFile"
echo "Make Install Release Log:" &>> "$LogFile"
echo "------------------------------" &>> "$LogFile"
$MakeInstall &>> "$LogFile"

echo "" &>> "$LogFile"
echo "Complete!" &>> "$LogFile"

echo ""
echo "Complete!"