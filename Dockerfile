# Use an official Node runtime as a parent image
FROM node:14-alpine

# Set the working directory in the container to /app
WORKDIR /app

# Copy package.json and package-lock.json to the workdir
COPY package*.json ./

# Install any needed packages specified in package.json
RUN npm install

# Copy the rest of your app's source code to the workdir
COPY . .

# Generate Prisma Client
RUN npx prisma generate

# Make port 3000 available to the world outside this container
EXPOSE 3000

# Run index.js when the container launches
CMD ["node", "index.js"]