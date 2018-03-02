# carOBD - Raspberry Pi Zero W

Describe tutorials to create a OBD Readaer based on Raspberry Pi Zero W.

### Step 1: Hardware required

- Raspberry Pi Zero W
- USB Cable with 2.0 A power supply + Car Supply
- Mini OBD-II, [ELM327 Bluetooth Adapter](https://produto.mercadolivre.com.br/MLB-709261876-mini-obd2-scanner-bluetooth-automotivo-para-carro-celular--_JM)
- Keyboard and mouse USB


### Step 2: Software required

1) Set up the Raspberry Pi

Follow this guide: https://www.raspberrypi.org/documentation/installation/installing-images/

The software requires a Linux-based operating system such as Ubuntu or Raspbian.

2) Before proceeding, run:
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get autoremove
sudo reboot
```

3) Install these components using the command:
Install Python Serial package, BLE package and some Python 2.8 tools:

```
sudo apt-get install python-serial
sudo apt-get install bluetooth bluez-utils blueman
sudo apt-get install python-wxgtk2.8 python-wxtools wx2.8-i18n libwxgtk2.8-dev
```

Install Python OBD and Screen:
```
sudo pip install obd
sudo apt-get install screen
```

4) Next, download the OBD-Pi Software direct from GitHub
```
cd ~
git clone https://github.com/Pbartek/pyobd-pi.git
```
