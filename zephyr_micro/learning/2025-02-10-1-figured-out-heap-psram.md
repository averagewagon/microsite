### **PSRAM Configuration in Zephyr (ESP32-S3)**

1. Followed
   [Espressif’s instructional](https://developer.espressif.com/blog/2024/12/zephyr-how-to-use-psram/)
   to configure PSRAM.
2. **Chose not to add PSRAM to the system heap**, instead left it as a
   **separate region in the multiheap** (`shared_multi_heap`).
3. Encountered a log message indicating **actual PSRAM size was smaller than
   configured**.
4. **Solution:**
   - Searched the ESP32 HAL for how it detects PSRAM size.
   - Manually printed the detected size.
   - Updated `prj.conf` to set `CONFIG_ESP_SPIRAM_SIZE` to exactly the detected
     value.

---

### **Flash Memory & Devicetree in Zephyr**

1. **Understanding Flash Controller vs. Flash Memory**

   - **`flash-controller@60002000`** (Compatible:
     `"espressif,esp32-flash-controller"`) → **Device**
   - **`flash@0` inside `flash-controller`** (Compatible: `"soc-nv-flash"`) →
     **Not a device**, just a **memory-mapped region**.

2. **Why `soc-nv-flash` Isn't a Device**

   - `soc-nv-flash` is just a **memory description**, not a runtime object.
   - No `DEVICE_DEFINE()` or `DEVICE_DT_DEFINE()` registers it.
   - Only **flash controllers** are treated as devices in Zephyr.

3. **Determining if a Node is a Zephyr Device**

   - **Look up `compatible` values** in Zephyr source code.
   - **Check `devicetree_unfixed.h`**
     (`build/zephyr/include/generated/devicetree_unfixed.h`):
     ```sh
     cat build/zephyr/include/generated/devicetree_unfixed.h | grep flash
     ```

4. **Platform-Independent Flash Access**
   - Instead of trying to access `soc-nv-flash`, use:
     ```c
     DEVICE_DT_GET(DT_PARENT(DT_NODELABEL(flash0)))
     ```
   - Use the **Zephyr Flash API (`flash_read()`, `flash_write()`,
     `flash_erase()`)** to access flash safely across different platforms.

---

### **Key Takeaways**

- **PSRAM:** Configured separately in the multiheap, with the exact detected
  size.
- **Flash:** `flash-controller` is the device; `flash@0` is a memory region.
- **Devicetree & Devices:** Devices exist only if registered via
  `DEVICE_DEFINE()`.
- **Verification:** `devicetree_unfixed.h`, `DEVICE_DT_GET()`, or Zephyr shell
  (`dt path`).
