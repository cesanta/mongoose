# See https://docs.zephyrproject.org/latest/develop/getting_started/index.html
# for script details

ZEPHYR_SDK_VER=0.14.0

cd ~
sudo apt install -y wget 
rm -rf kitware-archive.sh
wget https://apt.kitware.com/kitware-archive.sh
sudo bash kitware-archive.sh
sudo apt install --no-install-recommends -y git cmake ninja-build gperf ccache dfu-util device-tree-compiler wget python3-dev python3-pip python3-setuptools python3-tk python3-wheel xz-utils file make gcc gcc-multilib g++-multilib libsdl2-dev
pip3 install --user -U west
export PATH=~/.local/bin:"$PATH"
rm -rf ~/zephyrproject
west init ~/zephyrproject
cd ~/zephyrproject
west update
west zephyr-export
pip3 install --user -r ~/zephyrproject/zephyr/scripts/requirements.txt
cd ~
rm -rf zephyr-sdk-${ZEPHYR_SDK_VER}_linux-x86_64.tar.gz
echo Downloading zephyr-sdk-${ZEPHYR_SDK_VER}
wget -q https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v${ZEPHYR_SDK_VER}/zephyr-sdk-${ZEPHYR_SDK_VER}_linux-x86_64.tar.gz
tar xf zephyr-sdk-${ZEPHYR_SDK_VER}_linux-x86_64.tar.gz
cd zephyr-sdk-${ZEPHYR_SDK_VER}
./setup.sh -c
