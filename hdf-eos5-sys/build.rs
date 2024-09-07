extern crate bindgen;
extern crate cc;

use std::env;
use std::path::PathBuf;

fn main() {
    // Step 1: Build the HDF-EOS5 C library
    let src_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("hdf-eos5-2.0");
    
    cc::Build::new()
        .file("hdf-eos5-2.0/gctp/src/gctp.c") // Example: Add more .c files here as needed
        .file("hdf-eos5-2.0/src/other_source_files.c") // Add other source files
        .include("hdf-eos5-2.0/include") // Add the include directory
        .compile("hdf_eos5");

    // Step 2: Generate Rust bindings with bindgen
    let bindings = bindgen::Builder::default()
        .header("wrapper.h")
        .clang_arg("-Ihdf-eos5-2.0/include") // Tell bindgen where to find the headers
        .generate()
        .expect("Unable to generate bindings");

    // Step 3: Write the bindings to the $OUT_DIR
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");

    // Step 4: Link the compiled library to the Rust code
    println!("cargo:rustc-link-lib=dylib=hdf_eos5");
    println!("cargo:rustc-link-lib=dylib=hdf5"); // Ensure HDF5 is linked as well
}
