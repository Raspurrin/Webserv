
pub async fn test_delete(client: &reqwest::Client, filename: String) -> Result<(), &'static str> {
	let res = client.delete(format!("http://localhost:8080/upload/{}", filename)).send().await;
	match res {
		Ok(ret) => if ret.status() != 200 {
			return Err("Could not delete file")
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.delete("http://localhost:8080/dir/index.html").send().await;
	match res {
		Ok(ret) => if ret.status() != 405 {
			return Err("Could not delete file")
		},
		Err(_) => return Err("Failed to send request")
	}

	Ok(())
}