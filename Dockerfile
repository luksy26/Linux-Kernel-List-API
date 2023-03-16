FROM gitlab.cs.pub.ro:5050/so2/so2-assignments

RUN echo "Hello from Docker"

COPY ./checker ${CHECKER_DATA_DIRECTORY}
