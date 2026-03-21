"""import vfs
import machine, rp2


# Try to mount the filesystem, and format the flash if it doesn't exist.
# Note: the flash requires the programming size to be aligned to 256 bytes.
bdev = rp2.Flash()
try:
    fs = vfs.VfsLfs2(bdev, progsize=256)
except:
    vfs.VfsLfs2.mkfs(bdev, progsize=256)
    fs = vfs.VfsLfs2(bdev, progsize=256)
vfs.mount(fs, "/")

del vfs, bdev, fs
"""

import picocalc
from picocalc import PicoDisplay, PicoKeyboard, PicoSD, PicoSpeaker
import os
import vt
import sys
# Separated imports because Micropython is super finnicky
from picocalc_system import run, files, memory, disk

from pye import pye_edit

try:
    # Mount SD card to /sd on boot
    pc_sd = PicoSD()
    pc_sd.mount()
 
    _usb = sys.stdout  # 

    def usb_debug(msg):
        if isinstance(msg, str):
            _usb.write(msg)
        else:
            _usb.write(str(msg))
        _usb.write('\r\n')
    picocalc.usb_debug = usb_debug

    picocalc.sd = pc_sd

    os.dupterm(pc_terminal)
    pc_sd.check_mount()
    #usb_debug("boot.py done.")

except Exception as e:
    import sys
    sys.print_exception(e)
    try:
        os.dupterm(None).write(b"[boot.py error]\n")
    except:
        pass