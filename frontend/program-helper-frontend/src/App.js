// import React, { useState } from 'react';
// import ProblemList from './components/ProblemList';
// import ProblemDetail from './components/ProblemDetail';
// import TestCase from './components/TestCase';

// function App() {
//   const [selectedProblem, setSelectedProblem] = useState(null);

//   const problems = [
//     {
//       title: "Maximum Sum of Non-Adjacent Elements",
//       description: "Given an array of integers, return the maximum sum of non-adjacent elements.",
//       input: "An array of integers",
//       output: "Maximum sum of non-adjacent elements",
//     },
//     {
//       title: "Longest Increasing Subsequence",
//       description: "Given an array, find the length of the longest increasing subsequence.",
//       input: "An array of integers",
//       output: "Length of the longest increasing subsequence",
//     }
//   ];

//   const handleTestCaseSubmit = async (input) => {
//     try {
//       const response = await fetch('http://localhost:5000/api/solve', {
//         method: 'POST',
//         headers: { 'Content-Type': 'application/json' },
//         body: JSON.stringify({
//           prompt: input // Send the input as a prompt to OpenAI API
//         })
//       });
  
//       if (!response.ok) {
//         throw new Error('Network response was not ok');
//       }
  
//       const data = await response.json();
  
//       // Check if the response is an object with an error
//       if (data.error) {
//         return `Error: ${data.error}`;
//       }
  
//       return data.output;  // Get the AI response from the backend
//     } catch (error) {
//       console.error('Error:', error);
//       return `Error: ${error.message}`;
//     }
//   };

//   return (
//     <div className="container">
//       <div className="row mt-5">
//         <div className="col">
//           <h1 className="text-center">AI-Enhanced Competitive Programming Helper</h1>
//         </div>
//       </div>
//       <div className="row mt-4">
//         {!selectedProblem ? (
//           <div className="col-md-6 offset-md-3">
//             <ProblemList problems={problems} onSelect={setSelectedProblem} />
//           </div>
//         ) : (
//           <div className="col-md-8 offset-md-2">
//             <ProblemDetail problem={selectedProblem} />
//             <TestCase onSubmit={handleTestCaseSubmit} />
//           </div>
//         )}
//       </div>
//     </div>
//   );
// }

// export default App;

import React, { useState } from 'react';
import ProblemList from './components/ProblemList';
import ProblemDetail from './components/ProblemDetail';
import TestCase from './components/TestCase';

function App() {
  const [selectedProblem, setSelectedProblem] = useState(null);
  const [solution, setSolution] = useState("");
  const [testCases, setTestCases] = useState([]);
  const [userCode, setUserCode] = useState("");
  const [evaluation, setEvaluation] = useState("");

  const problems = [
    {
      title: "Maximum Sum of Non-Adjacent Elements",
      description: "Given an array of integers, return the maximum sum of non-adjacent elements.",
      input: "An array of integers",
      output: "Maximum sum of non-adjacent elements",
    },
    // Additional problems
  ];

  const handleTestCaseSubmit = async (input) => {
    try {
      const response = await fetch('http://localhost:5000/api/solve', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          prompt: input
        })
      });
  
      if (!response.ok) {
        throw new Error('Network response was not ok');
      }
  
      const data = await response.json();
      setSolution(data.solution);
      setTestCases(data.test_cases);  // Display test cases in frontend
    } catch (error) {
      console.error('Error:', error);
    }
  };

  const handleCodeSubmit = async () => {
    try {
        const payload = {
            prompt: selectedProblem.title,
            code: userCode
        };

        console.log("Payload being sent:", payload);  // Log the payload being sent

        const response = await fetch('http://localhost:5000/api/solve', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(payload)  // Send the payload as JSON
        });

        const responseText = await response.text();  // Fetch raw text
        console.log("Raw response text:", responseText);  // Log the raw text

        let data;
        try {
            data = JSON.parse(responseText);
        } catch (parseError) {
            console.error("Error parsing JSON:", parseError.message);
            console.error("Raw response that failed to parse:", responseText);
            throw new Error("Invalid JSON response from server");
        }

        console.log("Parsed result from backend:", data);

        // Display the evaluation field (assuming setEvaluation updates the UI)
        setEvaluation(data.evaluation);

    } catch (error) {
        console.error('Error during code submission:', error);
    }
};

  return (
    <div className="container mt-5">
      <div className="row">
        <div className="col text-center">
          <h1>AI-Enhanced Competitive Programming Helper</h1>
        </div>
      </div>

      <div className="row mt-4">
        {!selectedProblem ? (
          <div className="col-md-6 offset-md-3">
            <ProblemList problems={problems} onSelect={setSelectedProblem} />
          </div>
        ) : (
          <div className="col-md-8 offset-md-2">
            <ProblemDetail problem={selectedProblem} />

            <div className="mt-4">
              <h2>Solution</h2>
              <div className="alert alert-info">{solution}</div>

              <h2>Test Cases</h2>
              <ul className="list-group">
                {testCases.map((testCase, index) => (
                  <li key={index} className="list-group-item">
                    Test Case {index + 1}: {testCase}
                  </li>
                ))}
              </ul>

              <h2 className="mt-4">Your Code</h2>
              <textarea
                className="form-control"
                value={userCode}
                onChange={(e) => setUserCode(e.target.value)}
                rows="10"
                placeholder="Enter your code here"
              />

              <button className="btn btn-primary mt-3" onClick={handleCodeSubmit}>
                Submit Code
              </button>

              <h2 className="mt-4">Evaluation Results</h2>
              <div className={`alert ${evaluation.includes("Failed") ? 'alert-danger' : 'alert-success'}`}>
                {evaluation}
              </div>
            </div>
          </div>
        )}
      </div>
    </div>
  );
}

export default App;
