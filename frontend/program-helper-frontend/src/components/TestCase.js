import React, { useState } from 'react';
import { Button, Form } from 'react-bootstrap';

function TestCase({ onSubmit }) {
  const [testCaseInput, setTestCaseInput] = useState('');
  const [testCaseOutput, setTestCaseOutput] = useState(null);

  const handleSubmit = async () => {
    const output = await onSubmit(testCaseInput);
    setTestCaseOutput(output);
  };

  return (
    <div className="test-case">
      <Form>
        <Form.Group>
          <Form.Label>Enter Test Case</Form.Label>
          <Form.Control
            as="textarea"
            rows={3}
            value={testCaseInput}
            onChange={(e) => setTestCaseInput(e.target.value)}
            placeholder="Enter test case input"
          />
        </Form.Group>
        <Button variant="primary" onClick={handleSubmit} className="mt-3">
          Submit
        </Button>
      </Form>

      {testCaseOutput && (
        <div className="test-case-output">
        <h4>Output:</h4>
        <p>
          {typeof testCaseOutput === 'string'
            ? testCaseOutput
            : JSON.stringify(testCaseOutput)} {/* Ensure object is converted to string */}
        </p>
      </div>
      )}
    </div>
  );
}

export default TestCase;