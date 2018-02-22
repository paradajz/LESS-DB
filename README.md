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

## Async update

If `DBMS_ENABLE_ASYNC_UPDATE` symbol is specified, asynchronous data writing can be enabled. By default, calling `update` function results in immediate write to calculated memory address. If your application requires writing of large amount of data at certain points, async writing can be used by appending `true` as additional parameter to `update` function. If async write is requested, data is stored into internal buffer first, and then it's written to EEPROM by calling `checkQueue` function which writes single data value at the time, thus avoiding blocking your application due to slow EEPROM write access. In order to use async write, `DBMS_UPDATE_BUFFER_SIZE` symbol must also be specified and set to certain value which defines size of internal buffer.

## Example

An example database can be found in example directory.