## **Useful Commands**

Quick reference for debugging and development commands in Zephyr.

- **Flashing and Monitoring**

  - `west flash && west espressif monitor`
    - Flash firmware and start serial monitoring.

- **Devicetree Debugging**

  - `grep -A 5 'flash@0' build/zephyr/zephyr.dts.pre`
    - Look at the first 5 lines after `flash@0` in the preprocessed DTS file.
  - `grep -A 30 'flash@0' build/zephyr/zephyr.dts`
    - View 30 lines after `flash@0` in the final compiled DTS.
  - `cat build/zephyr/include/generated/zephyr/devicetree_generated.h`
    - Print the generated devicetree header file for C macros.
  - `cat build/zephyr/zephyr.dts`
    - Print the final compiled devicetree.
  - `cat /home/johnrhen/zephyrproject/zephyr/boards/espressif/esp32s3_devkitm/esp32s3_devkitm_procpu.dts`
    - Print the board-specific DTS file.

- **Building and Configuration**
  - `west build -p always`
    - Clean and rebuild from scratch.
  - `west build -t menuconfig`
    - Open the Kconfig menu configuration tool.

---

## **Zephyr Debugging Notes**

### **Key Takeaways About Zephyr**

- **Devicetree governs everything** – Board hardware is described in devicetree
  files (`.dts` and `.dtsi`), and this is used to generate C macros for access.
- **Generated Headers Matter** – The compiled `devicetree_generated.h` contains
  the processed results, and if something doesn’t exist there, it’s likely not
  accessible.
- **Device Drivers are Enabled via Kconfig** – Just because hardware is present
  in DTS doesn’t mean a driver exists; it must be enabled in Kconfig.
- **Flash is Not Straightforward** – Even though flash appears in DTS, the
  controller and memory must be properly referenced and supported.
- **DEVICE_DT_GET() Requires a Valid Node** – If `DEVICE_DT_GET()` fails to
  compile, it means the node isn’t properly defined or lacks a registered
  driver.

### **Key Problems and Lessons**

#### **Understanding Undefined References (`__device_dts_ord_<N>`)**

- This error occurs when a `DEVICE_DT_GET()` references a node that does not
  have an associated driver in the build.
- Fixes include:
  - Ensuring `status = "okay";` in the devicetree.
  - Enabling the correct Kconfig options for the driver.
  - Checking `devicetree_generated.h` to confirm node existence.

#### **Why Didn’t Flash Work Initially?**

- The original attempt to access flash (`DEVICE_DT_GET(DT_NODELABEL(flash0))`)
  failed because **`flash0` was not a valid node** that had a registered device
  in Zephyr.
- We found the flash **controller** was actually registered as
  `flash-controller@60002000`, so we had to switch to using
  `DEVICE_DT_GET_ANY(espressif_esp32_flash_controller)`.
- Using `flash_get_parameters()`, we confirmed flash existed and retrieved block
  sizes.

#### **Challenges with Determining Flash Size**

- Using `flash_get_page_count()` and `flash_get_page_info_by_idx()`, we
  calculated a flash size of ~32MB, which **was incorrect**.
- Likely cause: **Non-uniform page sizes** in external flash may have led to
  miscalculation.
- **Next steps**:
  - Investigate how Zephyr handles multiple flash regions (internal + external).
  - Cross-check with hardware specifications.

---

## **Discovery Log: How We Got Here**

We started with an **undefined reference error (`__device_dts_ord_<N>`)** when
trying to access flash. This led us down a path of debugging **devicetree
overlays**, checking **generated headers**, and **confirming device
registration**.

1. **Initial Debugging**:

   - We tried `DEVICE_DT_GET(DT_NODELABEL(flash0))` but got a compiler error.
   - Checked `devicetree_generated.h` and found no `flash0` entry.
   - Confirmed that `flash-controller@60002000` **was registered**, so we
     switched to `DEVICE_DT_GET_ANY(espressif_esp32_flash_controller)`.

2. **Confirming Flash Parameters**:

   - Used `flash_get_parameters()` to check **write block size** and **erase
     value**.
   - This verified that the flash driver was **active** and working.

3. **Flash Size Investigation**:

   - Used `flash_get_page_count()` and `flash_get_page_info_by_idx()` to
     calculate the flash size.
   - The reported size (~32MB) **didn’t match** the expected hardware limits
     (~16MB external + internal ESP32 flash).
   - This led to the realization that **non-uniform page sizes or incorrect
     driver assumptions** may have affected the results.

4. **Unanswered Questions & Next Steps**:
   - How does Zephyr handle multiple flash banks (internal + external)?
   - Are we missing a configuration flag for accurate size reporting?
   - Can we cross-check the reported flash size with ESP-IDF tools?
