import React, { useState } from 'react';

function TestCase({ onSubmit }) {
  const [testCaseInput, setTestCaseInput] = useState('');
  const [testCaseOutput, setTestCaseOutput] = useState(null);

  const handleSubmit = async () => {
    const output = await onSubmit(testCaseInput);
    setTestCaseOutput(output);
  };

  return (
    <div className="test-case">
      <h3>Enter Test Case</h3>
      <textarea
        value={testCaseInput}
        onChange={(e) => setTestCaseInput(e.target.value)}
        placeholder="Enter test case input"
      />
      <button onClick={handleSubmit}>Submit</button>

      {testCaseOutput && (
        <div className="test-case-output">
          <h4>Output:</h4>
          <p>{testCaseOutput}</p>
        </div>
      )}
    </div>
  );
}

export default TestCase;