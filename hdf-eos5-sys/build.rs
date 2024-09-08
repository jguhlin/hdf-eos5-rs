extern crate bindgen;
extern crate cc;
extern crate hdf5_sys;

use std::env;
use std::path::PathBuf;

fn main() {
    // Step 1: Build the HDF-EOS5 C library
    let src_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("hdf-eos5-2.0");

    // Print the env for debugging
    for (key, value) in env::vars() {
        println!("{}: {}", key, value);
    }

    let hdf5_include = env::var("DEP_HDF5_INCLUDE").expect("Set HDF5_INCLUDE_DIR environment variable to HDF5 include path");
    let hdf5_lib_dir = env::var("DEP_HDF5_ROOT").expect("Set HDF5_LIB_DIR environment variable to HDF5 library path");
    
    cc::Build::new()
        // .file("hdf-eos5-2.0/gctp/src/gctp.c") // Example: Add more .c files here as needed
        .include("hdf-eos5-2.0/include")
        .include(hdf5_include.clone())  // Include HDF5 headers

        .file("hdf-eos5-2.0/src/EHapi.c")
        .file("hdf-eos5-2.0/src/GDapi.c")
        .file("hdf-eos5-2.0/src/HE5common.c")
        .file("hdf-eos5-2.0/src/PTapi.c")
        .file("hdf-eos5-2.0/src/SWapi.c")
        .file("hdf-eos5-2.0/src/TSapi.c")
        .file("hdf-eos5-2.0/src/ZAapi.c")
        .compile("hdf_eos5");

    // Step 2: Generate Rust bindings with bindgen
    let bindings = bindgen::Builder::default()
        .header("wrapper.h")
        .clang_arg("-Ihdf-eos5-2.0/include") // Tell bindgen where to find the headers
        .clang_arg(format!("-I{}", hdf5_include)) // Tell bindgen where to find the HDF5 headers
        .blocklist_item("FP_NAN")
        .blocklist_item("FP_INFINITE")
        .blocklist_item("FP_ZERO")
        .blocklist_item("FP_SUBNORMAL")
        .blocklist_item("FP_NORMAL")
        .generate()
        .expect("Unable to generate bindings");

    // Step 3: Write the bindings to the $OUT_DIR
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");

    // Step 4: Link the compiled library to the Rust code
    println!("cargo:rustc-link-search=native={}", hdf5_lib_dir); // HDF5 lib dir from env
    println!("cargo:rustc-link-lib=dylib=hdf_eos5");
    println!("cargo:rustc-link-lib=dylib=hdf5"); // Ensure HDF5 is linked as well
}
