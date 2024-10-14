import React from 'react';
import { ListGroup } from 'react-bootstrap';

function ProblemList({ problems, onSelect }) {
  return (
    <div className="problem-list">
      <h2 className="text-center mb-4">Select a Problem</h2>
      <ListGroup>
        {problems.map((problem, index) => (
          <ListGroup.Item
            key={index}
            action
            onClick={() => onSelect(problem)}
            className="text-center"
          >
            {problem.title}
          </ListGroup.Item>
        ))}
      </ListGroup>
    </div>
  );
}

export default ProblemList;