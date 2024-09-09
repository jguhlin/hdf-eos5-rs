pub use hdf_eos5_sys::*;

pub fn start() {
    let data = "/mnt/data/development/hdf-eos5-rs/sample_data/MOD07_L2.A2024252.0000.061.2024252131104.hdf";
    // Convert to C string
    let data = std::ffi::CString::new(data).unwrap();
    let fileid = unsafe { HE5_GDopen(data.as_ptr() as *const i8, 
        HE5F_ACC_RDONLY) };
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_open_file() {
        start();
    }
}
