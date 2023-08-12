mod get;
mod post;
mod delete;
mod cgi;
mod unknown;

use std::io::Read;
use crate::delete::test_delete;
use crate::post::test_post;
use crate::get::test_get;
use crate::cgi::test_cgi;
use crate::unknown::test_unknown;
use reqwest;
use color_print::cprintln;
use random_string::generate;

#[tokio::main]
async fn main() -> std::result::Result<(), Box<dyn std::error::Error>> {

	let client = reqwest::Client::new();
	let upload_name = generate(5, "0123456789");
	evaluate_test("Get", test_get(&client).await);
	evaluate_test("Post", test_post(&client, upload_name.clone()).await);
	evaluate_test("Delete", test_delete(&client, upload_name.clone()).await);
	evaluate_test("Cgi", test_cgi(&client, upload_name.clone()).await);
	evaluate_test("Unknown Request", test_unknown(&client).await);
	evaluate_test("Timeout", test_timeout().await);

	Ok(())
}

fn evaluate_test(test_name: &str, test_output: Result<(), &str>) {
	print!("{} Test: ", test_name);
	match test_output {
		Ok(()) => cprintln!("<green>passed</green>"),
		Err(err) => cprintln!("<red>failed. {}</red>", err)
	}
}

async fn test_timeout() -> Result<(), &'static str> {
	let con = std::net::TcpStream::connect("127.0.0.1:8081");
	match con {
		Ok(mut con) => {
			tokio::time::sleep(tokio::time::Duration::new(3, 0)).await;
			let mut output = String::new();
			if let Err(_) =  con.read_to_string(&mut output) {
				return Err("Could not read from stream")
			}
			if let None = output.find("408") {
				return Err("Did not find timout response code")
			}
		},
		Err(_) => return Err("Could not connect to server")
	}
	Ok(())
}
