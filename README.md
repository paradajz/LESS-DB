# Simple database-like management system for AVR

This module simulates database using internal EEPROM memory on AVR microcontrollers. DBMS takes care of accessing, writing and organizing data into EEPROM, so no knowledge of memory addressing is required for the user.

## Data organization

DBMS is using blocks and sections with various parameters to organize data in EEPROM memory. Image
below displays possible internal data organization in EEPROM memory using DBMS.

![](https://raw.githubusercontent.com/paradajz/AVR-DB/master/img/memory.png)

### Blocks

A block of data is specified using the following parameters:

- Number of sections
- Section

### Sections

Block section is specified using the following parameters:

- Total number of parameters inside section
- Data parameter type (Bit, byte, word or dword)
- Preserve on partial reset (if set to true, data in section won't be cleared when performing reset of data)
- Default value (value which will be assigned to all parameters inside section)
- Auto increment (if set to true, default value will be used as starting value for first parameter, and all consecutive parameters will be incremented by 1)

## Symbols

In order to use this module, several symbols must be specified.

### `DBMS_MAX_SECTIONS`

This symbol must be set to wanted value in order to define maximum amount of database sections.

### `DBMS_MAX_BLOCKS`

This symbol must be set to wanted value in order to define maximum amount of database sections.

## Example

An example database can be found in example directory.