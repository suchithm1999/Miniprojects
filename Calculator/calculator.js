const keys = document.querySelectorAll("#key");
const operations = document.querySelectorAll("#operation");
const allClear = document.querySelector("#clear");
const del = document.querySelector("#delete");
const evaluate = document.querySelector("#equal");
const displayUnit = document.querySelector("#output");
let displayText = "";
let operatorValue = "";

display = (val) => {
  console.log(val);
  displayUnit.innerHTML = val;
};

addition = (num1, num2) => {
  return num1 + num2;
};

substraction = (num1, num2) => {
  return num1 - num2;
};

multiplication = (num1, num2) => {
  return num1 * num2;
};

division = (num1, num2) => {
  if (num2 === 0) {
    return "Invalid Operation";
  } else {
    return num1 / num2;
  }
};

modulo = (num1, num2) => {
  return num1 % num2;
};

for (let key of keys) {
  key.addEventListener("click", (e) => {
    displayText += e.target.value;
    display(displayText);
  });
}

let flag = 0;

for (let operator of operations) {
  operator.addEventListener("click", (e) => {
    operatorValue += e.target.value;
    displayText += e.target.value;
    display(displayText);
    flag += 1;
    if (flag > 1) {
      display("Multi Operations not allowed");
      displayText = "";
      flag = 0;
      operatorValue = "";
    }
  });
}

allClear.addEventListener("click", () => {
  displayText = "";
  display(displayText);
  operatorValue = "";
  flag = 0;
});

del.addEventListener("click", () => {
  if (displayText[displayText.length - 1] === operatorValue) {
    operatorValue = "";
    flag = 0;
    displayText = displayText.slice(0, -1);
  } else {
    displayText = displayText.slice(0, -1);
  }
  display(displayText);
});

let result = 0;

evaluate.addEventListener("click", () => {
  let numbers = displayText.split(operatorValue);
  num1 = parseFloat(numbers[0]);
  num2 = parseFloat(numbers[1]);
  switch (operatorValue) {
    case "+":
      result = addition(num1, num2);
      display(result);
      break;
    case "-":
      result = substraction(num1, num2);
      display(result);
      break;
    case "\u00d7":
      result = multiplication(num1, num2);
      display(result);
      break;
    case "\u00f7":
      result = division(num1, num2);
      display(result);
      break;
    case "%":
      result = modulo(num1, num2);
      display(result);
      break;
    default:
      display("Invalid Operation");
  }
  operatorValue = "";
  displayText = result.toString();
  flag = 0;
});
