# **Microsite MCU-Specific Code**

This is an ESP-IDF-based project for hosting a lightweight static website on an
ESP32 microcontroller. Microsite organizes its code into modular components to
ensure clarity and maintainability in resource-constrained environments.

---

## **Style Conventions**

### **1. Folder Structure**

Organize code into modular directories with public headers exposing only the
necessary interfaces. Private implementation details are kept in the same folder
but not exposed externally.

```
├── jms
│   ├── error
│   │   └── error.h
│   ├── filesystem
│   ├── mime
│   │   ├── mime.gperf
│   │   ├── mime.h
│   │   ├── mime_hash.c
│   │   └── priv_mime_hash.h
│   └── web_server
├── Kconfig.projbuild
└── main.c
```

---

### **2. File and Function Naming**

- **File Names**: Use lowercase words separated by underscores (e.g.,
  `mime_hash.c`, `error.h`).
- **Function Names**: Use `<module>_<submodule>_<action>` to indicate the module
  of origin.

```c
// Examples
jms_mime_getType(); // Public API
in_word_set();      // Private gperf-generated function
```

---

### **3. Includes**

Includes should reflect the folder structure for clarity and organization.

```c
#include "jms/error/error.h"
#include "jms/mime/mime.h"
```

---

### **4. Error Handling**

- Use `jms_err_t` (a `uint16_t`) for error codes.
- Define error codes as macros prefixed with `JMS_`.

```c
typedef uint16_t jms_err_t;

#define JMS_OK                 ((jms_err_t)(0))
#define JMS_ERR_UNKNOWN        ((jms_err_t)(0x1001))
#define JMS_ERR_INVALID_ARG    ((jms_err_t)(0x1002))
#define JMS_ERR_MIME_NOT_FOUND ((jms_err_t)(0x5001))
```

- Use the `JMS_HANDLE_ERR` macro to log and assert on errors.

```c
#define JMS_HANDLE_ERR(tag, jms_err_code)                              \
    do {                                                              \
        if ((jms_err_code) > JMS_OK) {                                \
            ESP_LOGE(tag, "Error Code: 0x%04X | File: %s | Line: %d", \
                     (jms_err_code), __FILE__, __LINE__);             \
            assert((jms_err_code) == JMS_OK);                         \
        }                                                             \
    } while (0)
```

---

## **gperf Usage**

### **Workflow**

1. Edit the `mime.gperf` file in `jms/mime/` to update the MIME types or file
   extensions.
2. Run the following command to regenerate the hash file:
   ```bash
   gperf jms/mime/mime.gperf > jms/mime/mime_hash.c
   ```
3. Ensure the generated `mime_hash.c` is included in the build system.

---

This guide provides the basic conventions for maintaining clarity, consistency,
and modularity in the project.
