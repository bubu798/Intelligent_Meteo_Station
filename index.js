const express = require("express");
const prisma = require("./prisma/prisma");
const cors = require("cors");
const app = express();

app.use(cors());
app.use(express.text());

function parseData(body) {
  const value = body.split(" ").filter(Boolean);
  return [value[0].slice(0, -1), Number(value[1])];
}

app.post("/", async (req, res) => {
  const body = parseData(req.body);
  console.log(body);
  await prisma.data.create({
    data: {
      type: body[0],
      value: body[1],
    },
  });
  res.status(200).send("ok");
});

app.get("/", async (req, res) => {
  const data = await prisma.data.findMany({
    take: 3000,
    orderBy: {
      id: "desc",
    },
  });
  res.status(200).json(data.reverse());
});

app.listen(3000, () => console.log("Server started on port 3000"));
