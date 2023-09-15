RDF Simple Decoder
==================

## Requirements

- ROOT >= v6.20.00

## Usage

Clone this repository.

```sh
git clone https://github.com/yano404/RDFSimpleDecoder.git
```

Edit `src/main.cc` and then build decoder.

```sh
cd RDFSimpleDecoder
make
```

Link data directory.

```sh
ln -s /path/to/rdf ./rdf
```

Decode RDF.
The output file `root/<run number>.root` will be created.

```sh
./decode <run number>
```

## License

Copyright (c) 2023 Takayuki YANO

The source code is licensed under the LGPL 2.1 License, see LICENSE.
