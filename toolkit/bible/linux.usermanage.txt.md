
# linux user management
# Add a user named Mike
First, change to root and use the command
```
useradd -m -s /bin/bash Mike
```
    -m  create user's home directory;
    -s  appoints user's shell type, such as sh, csh, tcsh or bash;

``` 
useradd -m -s /bin/bash -G sudo Mike
```
> -G  add Mike to another group(s);

```
passwd Mike
```
> to set user's password, if you don't do this step, the user can't login the system;

If you want the new user Mike could use "sudo", you need to modify the file "/etc/sudoers", or use below command
for Ubuntu
```
gpasswd -a Mike sudo
```

for CentOS
```
gpasswd -a Mike wheel
```
> then modify /etc/sudoers, uncomment #%wheel ALL=(ALL) ALL, means wheel group could oprate any machine by any user do any thing; 

# Delete the user Mike
Use the command
```
userdel -r Mike
```
> to delete the user and the home directory;

or
```
userdel Mike
```
> just delete user Mike but keep the home directory;

# Modify user's name User to Mike
```
usermod -l Mike -m -d /home/Mike User
```
> -l  modify user name User to Mike,
> -d  appoint to user directory /home/Mike and the directory /home/Mike should exist;
> -m  rename the old directory to the new one, if the old directory doesn't exist, then create it.

# Add/delete user to/from a second group
```
gpasswd -a user group2
```
> Add user to a second group

```
gpasswd -d user group2
```
> delete user from a second group

# groups operations
1.  create group
```
groupadd grp
```
> add a grp group
2. rename group
```
groupmod -n grp_new  grp_old
```
> rename group name grp_old to grp_new
3. delete group
```
groupdel grp
```
> delete group grp
4. list group
+ view current user belongs group(s);
```
groups
```
+ list user belongs group(s)
```
groups user
```

+ list all groups
```
cat /etc/group
```
> some system has no /etc/group file, use another way
```
cat /etc/passwd |awk -F [:] ‘{print $4}’ |sort|uniq | getent group |awk -F [:] ‘{print $1}’
```
> note that getent command could find group infomation by GID;

# user operations
## add user
```
useradd --help
Usage: useradd [options] LOGIN

Options:
  -b, --base-dir BASE_DIR       base directory for the home directory of the
                                new account
  -c, --comment COMMENT         GECOS field of the new account
  -d, --home-dir HOME_DIR       home directory of the new account
  -D, --defaults                print or change default useradd configuration
  -e, --expiredate EXPIRE_DATE  expiration date of the new account
  -f, --inactive INACTIVE       password inactivity period of the new account
  -g, --gid GROUP               name or ID of the primary group of the new
                                account
  -G, --groups GROUPS           list of supplementary groups of the new
                                account
  -h, --help                    display this help message and exit
  -k, --skel SKEL_DIR           use this alternative skeleton directory
  -K, --key KEY=VALUE           override /etc/login.defs defaults
  -l, --no-log-init             do not add the user to the lastlog and
                                faillog databases
  -m, --create-home             create the user's home directory
  -M, --no-create-home          do not create the user's home directory
  -N, --no-user-group           do not create a group with the same name as
                                the user
  -o, --non-unique              allow to create users with duplicate
                                (non-unique) UID
  -p, --password PASSWORD       encrypted password of the new account
  -r, --system                  create a system account
  -R, --root CHROOT_DIR         directory to chroot into
  -s, --shell SHELL             login shell of the new account
  -u, --uid UID                 user ID of the new account
  -U, --user-group              create a group with the same name as the user
  -Z, --selinux-user SEUSER     use a specific SEUSER for the SELinux user mapping
```

```
passwd user
```
> add a new user, must set user's passwd;

## Modify user's infomation
```
usermod
Usage: usermod [options] LOGIN
Options:
  -c, --comment COMMENT         new value of the GECOS field
  -d, --home HOME_DIR           new home directory for the user account
  -e, --expiredate EXPIRE_DATE  set account expiration date to EXPIRE_DATE
  -f, --inactive INACTIVE       set password inactive after expiration
                                to INACTIVE
  -g, --gid GROUP               force use GROUP as new primary group
  -G, --groups GROUPS           new list of supplementary GROUPS
  -a, --append                  append the user to the supplemental GROUPS
                                mentioned by the -G option without removing
                                him/her from other groups
  -h, --help                    display this help message and exit
  -l, --login NEW_LOGIN         new value of the login name
  -L, --lock                    lock the user account
  -m, --move-home               move contents of the home directory to the
                                new location (use only with -d)
  -o, --non-unique              allow using duplicate (non-unique) UID
  -p, --password PASSWORD       use encrypted password for the new password
  -R, --root CHROOT_DIR         directory to chroot into
  -s, --shell SHELL             new login shell for the user account
  -u, --uid UID                 new UID for the user account
  -U, --unlock                  unlock the user account
  -v, --add-subuids FIRST-LAST  add range of subordinate uids
  -V, --del-subuids FIRST-LAST  remvoe range of subordinate uids
  -w, --add-subgids FIRST-LAST  add range of subordinate gids
  -W, --del-subgids FIRST-LAST  remvoe range of subordinate gids
  -Z, --selinux-user SEUSER     new SELinux user mapping for the user account
```

```
usermod -d /home/user1 -G grp user
```
modify user's login dir to /home/user1, then join in grp group, should use -G;

```
gpasswd -a user group2
```
> add user to group group2

```
gpasswd -d user group2
```
> delete user from group group2

## delete user
```
userdel -r user
```
> delete user test

## list users
### list current login users
```
w
who
```
### list current user's name
```
whoami
```
### view user's information
```
finger apacheuser
id apacheuser
```
### view login log
```
last  #view users who login succeed;
lastb #view users who login failed;
```
### list all users
```
cut -d : -f 1 /etc/passwd
cat /etc/passwd | awk -F \: '{print $1}'
```
