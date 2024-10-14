import React from 'react';

function ProblemList({ problems, onSelect }) {
  return (
    <div className="problem-list">
      <h2>Select a Problem</h2>
      <ul>
        {problems.map((problem, index) => (
          <li key={index} onClick={() => onSelect(problem)}>
            {problem.title}
          </li>
        ))}
      </ul>
    </div>
  );
}

export default ProblemList;