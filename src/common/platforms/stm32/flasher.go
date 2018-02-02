// +build linux

package stm32

// #cgo CFLAGS: -I/usr/include/libusb-1.0/
// #cgo LDFLAGS: -lusb-1.0
// #include "stm32_flash.h"
import "C"

import (
	"time"
	"unsafe"

	"cesanta.com/mos/flash/common"
	"github.com/cesanta/errors"
)

type FlashOpts struct {
	ShareName string
	Timeout   time.Duration
}

func Flash(fw *common.FirmwareBundle, opts *FlashOpts) error {
	data, err := fw.GetPartData("boot")
	if err != nil {
		return errors.Annotatef(err, "invalid manifest")
	}

	flash_res := C.stm32_flash(C.CString(opts.ShareName),
		unsafe.Pointer(&data[0]),
		C.int(len(data)))

	if flash_res != 0 {
		return errors.Errorf("flash failed")
	}

	return nil
}
