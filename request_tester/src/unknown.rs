
pub async fn test_unknown(client: &reqwest::Client) -> Result<(), &str> {
	let res = client.put("http://localhost:8080/dir").send().await;
	match res {
		Ok(ret) => {
			if ret.status() != 501 {
				return Err("Expected status 501")
			}
		},
		Err(_) => return Err("Could not parse body")
	}

	let res = client.head("http://localhost:8080/dir").send().await;
	match res {
		Ok(ret) => {
			if ret.status() != 501 {
				return Err("Expected status 501")
			}
		},
		Err(_) => return Err("Could not parse body")
	}

	Ok(())
}
