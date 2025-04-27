# HLSCAM: High-Level Synthesis-Based Content Addressable Memory for Packet Processing

This repository contains the source code for **HLSCAM**, a flexible and high-performance Content Addressable Memory (CAM) framework implemented using **High-Level Synthesis (HLS)** for **FPGA-based packet processing**, available at: https://www.mdpi.com/2079-9292/14/9/1765

HLSCAM supports:
- **BCAM** (Binary CAM) for exact matching
- **TCAM** (Ternary CAM) for wildcard-based matching
- **STCAM** (Semi-TCAM) for prefix-based matching (Longest Prefix Match) (not investigated in the paper)

The design is templated and highly configurable, enabling easy adjustment of key width, value width, table depth, and optimization strategies.

## Features
- Written in standard **C++** for **AMD (Xilinx) Vitis HLS** toolchain
- Supports **Brute Force (BF)**, **Balanced (BL)**, and **High-Speed (HS)** optimization modes
- Flexible memory partitioning to balance between resource usage and lookup latency
- Achieves competitive operating frequencies without relying on BRAMs
- Suitable for **high-speed packet classification**, **firewall rule matching**, **flow table lookup**, and **5G core user management**

## Clone the repository:
   ```bash
   git clone https://github.com/abbasmollaei/HLSCAM
   ```
   
## Requirements
- AMD/Xilinx Vitis HLS 2023.2 or newer
- AMD/Xilinx Vivado for implementation and place-and-route evaluation

# How to cite the paper

@Article{electronics14091765,
AUTHOR = {Abbasmollaei, Mostafa and Ould-Bachir, Tarek and Savaria, Yvon},
TITLE = {HLSCAM: Fine-Tuned HLS-Based Content Addressable Memory Implementation for Packet Processing on FPGA},
JOURNAL = {Electronics},
VOLUME = {14},
YEAR = {2025},
NUMBER = {9},
ARTICLE-NUMBER = {1765},
URL = {https://www.mdpi.com/2079-9292/14/9/1765},
ISSN = {2079-9292},
ABSTRACT = {Content Addressable Memories (CAMs) are pivotal in high-speed packet processing systems, enabling rapid data lookup operations essential for applications such as routing, switching, and network security. While traditional Register-Transfer Level (RTL) methodologies have been extensively used to implement CAM architectures on Field-Programmable Gate Arrays (FPGAs), they often involve complex, time-consuming design processes with limited flexibility. In this paper, we propose a novel templated High-Level Synthesis (HLS)-based approach for the design and implementation of CAM architectures such as Binary CAMs (BCAMs) and Ternary CAMs (TCAMs) optimized for data plane packet processing. Our HLS-based methodology leverages the parallel processing capabilities of FPGAs through employing various design parameters and optimization directives while significantly reducing development time and enhancing design portability. This paper also presents architectural design and optimization strategies to offer a fine-tuned CAM solution for networking-related arbitrary use cases. Experimental results demonstrate that HLSCAM achieves a high throughput, reaching up to 31.18 Gbps, 9.04 Gbps, and 33.04 Gbps in the 256×128, 512×36, and 1024×150 CAM sizes, making it a competitive solution for high-speed packet processing on FPGAs.},
DOI = {10.3390/electronics14091765}
}
