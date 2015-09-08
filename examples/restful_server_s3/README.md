RESTful server with Amazon S3 upload example
============================================

This example demonstrates how Mongoose could be used to implement a RESTful
service that uses another RESTful service to handle it's own API call.
This example takes form data and uploads it as a file to Amazon S3.

## Prerequisites

- Amazon S3 account security credentials: Access Key ID and Secret Access
  Key ID. Get them from the Amazon IAM console.
- Amazon S3 bucket.

## Building and running the example

    $ git clone https://github.com/cesanta/mongoose.git
    $ cd mongoose/examples/restful_server_s3
    $ make
    $ ./restful_server_s3 -a ACCESS_KEY_ID -s SECRET_ACCESS_KEY_ID
    Starting RESTful server on port 8000

Then, open a browser on `http://localhost:8000`

Note: If you're getting a *Temporary Redirect* error, look what is the
Endpoint value is. It's likely that you have something like
`BUCKET_NAME.S3_ZONE.amazonaws.com`.
Change the *Host* field to `S3_ZONE.amazonaws.com and retry`.

## Screenshot

![](https://docs.cesanta.com/images/mongoose_s3_example.png)
