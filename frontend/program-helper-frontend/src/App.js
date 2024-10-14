import React, { useState } from 'react';
import ProblemList from './components/ProblemList';
import ProblemDetail from './components/ProblemDetail';
import TestCase from './components/TestCase';

function App() {
  const [selectedProblem, setSelectedProblem] = useState(null);

  const problems = [
    {
      title: "Maximum Sum of Non-Adjacent Elements",
      description: "Given an array of integers, return the maximum sum of non-adjacent elements.",
      input: "An array of integers",
      output: "Maximum sum of non-adjacent elements",
    },
    {
      title: "Longest Increasing Subsequence",
      description: "Given an array, find the length of the longest increasing subsequence.",
      input: "An array of integers",
      output: "Length of the longest increasing subsequence",
    }
  ];

  const handleTestCaseSubmit = async (input) => {
    // This is where you'd connect to the backend to send the test case
    // For now, we'll mock the backend response
    const response = await fetch('http://localhost:5000/api/solve', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        problem: selectedProblem.title,
        input: input
      })
    });

    const data = await response.json();
    return data.output;
  };

  return (
    <div className="App">
      <h1>AI-Enhanced Competitive Programming Helper</h1>
      {!selectedProblem ? (
        <ProblemList problems={problems} onSelect={setSelectedProblem} />
      ) : (
        <>
          <ProblemDetail problem={selectedProblem} />
          <TestCase onSubmit={handleTestCaseSubmit} />
        </>
      )}
    </div>
  );
}

export default App;
