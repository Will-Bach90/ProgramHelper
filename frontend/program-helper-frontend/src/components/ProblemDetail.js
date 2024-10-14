import React from 'react';
import { Card } from 'react-bootstrap';

function ProblemDetail({ problem }) {
  return (
    <Card className="mb-4">
      <Card.Body>
        <Card.Title>{problem.title}</Card.Title>
        <Card.Text>{problem.description}</Card.Text>
        <h5>Input:</h5>
        <Card.Text>{problem.input}</Card.Text>
        <h5>Output:</h5>
        <Card.Text>{problem.output}</Card.Text>
      </Card.Body>
    </Card>
  );
}

export default ProblemDetail;