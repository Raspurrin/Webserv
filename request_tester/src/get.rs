
pub async fn test_get(client: &reqwest::Client) -> Result<(), &'static str> {
	let res = client.get("http://localhost:8080/").send().await;
	if let Ok(ret) = res {
		if ret.status() != 200 {
			return Err("Got wrong status for /");
		}
	} else {
		return Err("Failed to send request");
	}

	let res = client.get("http://localhost:8080/asfasd").send().await;
	match res {
		Ok(ret) => if ret.status() != 404 {
			return Err("Got wrong status for random garbage")
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.get("http://localhost:8080/dir?test=test").send().await;
	match res {
		Ok(ret) => if ret.status() != 200 {
			return Err("Failed to separate query string")
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.get("http://localhost:8080/dir/garbage").send().await;
	match res {
		Ok(ret) => if ret.status() != 404 {
			return Err("Got wrong status for non existing file")
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.get("http://localhost:8080/dir/another/test").send().await;
	match res {
		Ok(ret) => if ret.status() != 200 {
			return Err("Failed to get nested directory")
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.get("http://localhost:8081/hello").send().await;
	match res {
		Ok(ret) => if ret.status() != 200 {
			return Err("Failed to get from another server")
		},
		Err(_) => return Err("Failed to send request")
	}

	let res = client.get("http://localhost:8080/hi").send().await;
	match res {
		Ok(ret) => if ret.status() != 405 {
			return Err("Method should not be allowed")
		},
		Err(_) => return Err("Failed to send request")
	}

	Ok(())
}
