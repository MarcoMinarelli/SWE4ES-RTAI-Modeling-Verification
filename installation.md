# Linux RTAI installation - in contruction
These are the steps I have done in order to install Linux RTAI on a x86 machine.
	0. Install a stable Ubuntu distro (in my case, 18.04); at this point it doesn't matter the Linux kernel version.
	1. Download a stable Linux RTAI version (in my case, 5.3).
	2. Unpack it under `/usr/src/ folder`.
	3. Under `rtai-x.y/base/arch/x86/patches/` there are a list of patches for various Linux (vanilla) kernels. Choose one (in my case, 4.19.177).
	4. Download the Linux kernel (https://mirrors.edge.kernel.org/pub/linux/kernel/) and unpack it under /usr/src/ folder.
	5. Move into the unpacked linux kernel and run `patch –p1 < /usr/src/rtai-x.y/base/arch/x86/patches/hal-linux-<version>-x86-[...].patch`
	6. make menuconfig
		There are a lot of possible choices; in order to make it run I had to deselect IPIPE and HYPERV automatic selection.
