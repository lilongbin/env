linux ubuntu clean

# remove unuseful packages
```
sudo apt autoremove
```

# remove unuseful packages
```
sudo du -sh /var/cache/apt
sudo apt autoclean # remove outdated packages
sudo apt clean     # remove download packages
```

# remove old kenerl
```
#uname -a
#dpkg --get-selections | grep linux-
#sudo apt remove --purge linux-
```

# purge app who has been removed
#sudo reboot; sudo apt autoremove
```
dpkg --list | grep '^rc' | cut -d ' ' -f 3 | xargs sudo dpkg --purge
```

```
sudo synaptic
```

```
sudo snap list --all
#sudo snap remove --revision=xxx yyy
```

