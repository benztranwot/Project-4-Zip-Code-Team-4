# ZIP Code Search System — Team 4

A C++ team project that organizes U.S. ZIP code records using a block sequence set and a tree index.

The application loads postal records from a file, builds an index of ZIP codes, and provides an interactive console for looking up place, state, and county information.

## Features

- Load postal records from a length-indicated data file.
- Store records in linked blocks.
- Build a ZIP code index using the included `BPlusTree` implementation.
- Search for a ZIP code through the console.
- Retrieve matching postal information from the block sequence set.
- Export the tree structure to `B+Tree_data.txt`.

The tree checks whether a ZIP code exists. Matching record details are then retrieved by traversing the block sequence set.

## Main Files

| File | Purpose |
|---|---|
| `B+tree_driver.cpp` | Application entry point and interactive search loop. |
| `B+tree.cpp` | Tree implementation, including insertion, search, deletion, and range-query methods. |
| `BlockPostalCode.h/.cpp` | Linked blocks containing postal records. |
| `BlockSequenceSetPostalCode.h/.cpp` | Manages the sequence of postal blocks. |
| `HeaderRecordPostalCodeItem.h/.cpp` | Stores postal fields and record length. |
| `PostalRecord.h` | Structure used to return search results. |
| `readHeaderPostalCodetoBSSBuffer.cpp` | Loads the formatted input file into the sequence set. |

## Requirements

A C++ compiler supporting C++17, such as GCC.

The application uses the C++ standard library and does not require third-party libraries.

## Build and Run

Open a terminal in the repository folder.

Compile the search application:

```bash
g++ -std=c++17 "B+tree_driver.cpp" BlockPostalCode.cpp BlockSequenceSetPostalCode.cpp HeaderRecordPostalCodeItem.cpp -o BPlusTreeApp.exe
```

Run it on Windows:

```powershell
.\BPlusTreeApp.exe
```

The driver directly includes `B+tree.cpp` and `readHeaderPostalCodetoBSSBuffer.cpp`, so those files are not listed separately in the compile command.

## Usage

Keep this input file in the working directory:

```text
us_postal_codes_length_indicated_header_record.txt
```

When the application starts, it loads the records, builds the index, and writes the tree structure to `B+Tree_data.txt`, replacing any existing contents.

Enter a ZIP code when prompted:

```text
Enter ZIP to search (0 to quit):
```

For a matching record, the application displays its ZIP code, place, state, and county. Enter `0` to exit.

## Documentation

The repository includes:

- `Design Document.docx`
- `Test Document.docx`
- `User Guide 4.docx`
- `doxygen generated PDF.pdf`

## License

Distributed under the MIT License. See [LICENSE](LICENSE) for details.
