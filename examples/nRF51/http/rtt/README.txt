README.txt for the SEGGER RTT Implementation Pack.
Date: 17 Dec 2014

Included files:
===============
Root Directory
  - Examples
    - RTT_MenuApp.c               - Sample application to demonstrate RTT bi-directional functionality. No OS.
    - RTT_embOS_SpeedTestApp.c    - Sample application for measuring RTT performance. embOS needed.
  - RTT
    - SEGGER_RTT.c                - The RTT implementation.
    - SEGGER_RTT.h                - Header for RTT implementation.
    - SEGGER_RTT_Conf.h           - Pre-processor configuration for the RTT implementation.
    - SEGGER_RTT_Printf.c         - Simple implementation of printf to write formatted strings via RTT.
  - Syscalls
    - RTT_Syscalls_GCC.c          - Low-level syscalls to retarget printf() to RTT with GCC / Newlib.
    - RTT_Syscalls_IAR.c          - Low-level syscalls to retarget printf() to RTT with IAR compiler.
    - RTT_Syscalls_KEIL.c         - Low-level syscalls to retarget printf() to RTT with KEIL/uVision compiler.