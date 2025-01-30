from framebuf import FrameBuffer, MONO_VLSB

class OLED_096:
    _CONTRAST        = 0x81
    _ENTIRE_ON       = 0xa4
    _NORM_INV        = 0xa6
    _DISP            = 0xae
    _MEM_ADDR        = 0x20
    _COL_ADDR        = 0x21
    _PAGE_ADDR       = 0x22
    _DISP_START_LINE = 0x40
    _SEG_REMAP       = 0xa0
    _MUX_RATIO       = 0xa8
    _COM_OUT_DIR     = 0xc0
    _DISP_OFFSET     = 0xd3
    _COM_PIN_CFG     = 0xda
    _DISP_CLK_DIV    = 0xd5
    _PRECHARGE       = 0xd9
    _VCOM_DESEL      = 0xdb
    _CHARGE_PUMP     = 0x8d
    def __init__(self, width, height, i2c, address = 0x3c, vcc = False):
        self.width = width
        self.height = height
        self.vcc = vcc
        self.i2c = i2c
        self.address = address
        self.pages = self.height // 8
        self.buffer = bytearray(self.pages * self.width)
        frame = FrameBuffer(self.buffer, self.width, self.height, MONO_VLSB)
        self.frame = frame
        self.fill = self._auto_show(frame.fill)
        self.pixel = self._auto_show(frame.pixel)
        self.hline = self._auto_show(frame.hline)
        self.vline = self._auto_show(frame.vline)
        self.line = self._auto_show(frame.line)
        self.rect = self._auto_show(frame.rect)
        self.fill_rect = self._auto_show(frame.fill_rect)
        self.text = self._auto_show(frame.text)
        self.scroll = self._auto_show(frame.scroll)
        self.blit = self._auto_show(frame.blit)
        self.init_display()
    def _auto_show(self, function):
        def wrapper(*args, **kwargs):
            result = function(*args, **kwargs)
            self.show()
            return result
        return wrapper
    def init_display(self):
        for cmd in (
            self._DISP | 0x00,
            self._MEM_ADDR, 0x00,
            self._DISP_START_LINE | 0x00,
            self._SEG_REMAP | 0x01,
            self._MUX_RATIO, self.height - 1,
            self._COM_OUT_DIR | 0x08,
            self._DISP_OFFSET, 0x00,
            self._COM_PIN_CFG, 0x02 if self.height == 32 else 0x12,
            self._DISP_CLK_DIV, 0x80,
            self._PRECHARGE, 0x22 if self.vcc else 0xf1,
            self._VCOM_DESEL, 0x30,
            self._CONTRAST, 0xff,
            self._ENTIRE_ON,
            self._NORM_INV,
            self._CHARGE_PUMP, 0x10 if self.vcc else 0x14,
            self._DISP | 0x01):
            self.write_cmd(cmd)
        self.fill(0)
        self.show()
    def off(self):
        self.write_cmd(self._DISP | 0x00)
    def on(self):
        self.write_cmd(self._DISP | 0x01)
    def contrast(self, contrast):
        self.write_cmd(self._CONTRAST)
        self.write_cmd(contrast)
    def invert(self, invert):
        self.write_cmd(self._NORM_INV | (invert & 1))
    def show(self):
        x0 = 0
        x1 = self.width - 1
        if self.width == 64:
            x0 += 32
            x1 += 32
        self.write_cmd(self._COL_ADDR)
        self.write_cmd(x0)
        self.write_cmd(x1)
        self.write_cmd(self._PAGE_ADDR)
        self.write_cmd(0)
        self.write_cmd(self.pages - 1)
        self.write_data(self.buffer)
    def write_cmd(self, cmd):
        self.i2c.writeto(self.address, bytearray([0x80, cmd]))
    def write_data(self, buf):
        self.i2c.writeto(self.address, bytearray([0x40]) + buf)