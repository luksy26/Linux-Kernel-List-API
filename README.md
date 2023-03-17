# README

## Setup
1. Login into your gitlab accout using the UPB login at https://gitlab.cs.pub.ro/users/sign_in
2. Make a **private** fork of this repository.
In the right corner you can find the fork button.
After clicking it you, the following should be prompted at you:

![private_fork](./img/private_fork.png)

Select your user as the namespace for your fork.
And please make sure the visibility level is set to **Private**

3. Clone your repo locally
```
$ git clone <remote_of_your_private_fork>
```

## Working on the assignment
Start the assignment from the skeleton inside the `src/` directory.
The first option is to code in the `src/` and run the checker using the command:
```
./local.sh checker 0-list
```
This will start the docker, start the SO2 VM inside the docker and run the checher for you inside the VM.

The second and recommended option is to connect to the docker interactively and then to the VM and run the checker manually.
1. Connect to the docker
```
./local.sh docker interactive
```
If you use native Linux you can benefit from KVM superpowers using the `--privileged` argument.
Thus the container will be started in privileged mode and we have access to KVM.
After executing the last command you should get a prompter in the docker.
When running in interactive mode the `src/` directory is mounted to the `/linux/tools/labs/skels/assignments/0-list`.
So any change in one would affect the other.
**Be careful not to delete your code!**.
We encourage you to make as many commits as possible on gitlab!

2. Build your assignment:
```
# you should be in /linux/tools/labs
make build
```

3. You can start the VM with the following command:
```
make console
```
After this command you should get a prompter inside the VM

4. Copy the *.ko into the skels/assginments/0-list-checker
```
cd skels/assignments/0-list-checker
cp ../0-list/list.ko . 
```

5. Run the checker
```
# you should be in skels/assignments/0-list-checker
sh _checker
```

# Submiting the assignment
TODO