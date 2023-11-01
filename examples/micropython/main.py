import mongoose

mongoose.init()
while True:
    mongoose.poll(1000)
