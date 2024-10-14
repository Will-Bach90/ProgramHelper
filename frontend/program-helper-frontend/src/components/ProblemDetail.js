import React from 'react';

function ProblemDetail({ problem }) {
  return (
    <div className="problem-detail">
      <h2>{problem.title}</h2>
      <p>{problem.description}</p>
      <h4>Input:</h4>
      <p>{problem.input}</p>
      <h4>Output:</h4>
      <p>{problem.output}</p>
    </div>
  );
}

export default ProblemDetail;